#!/bin/bash
kubectl get pods --no-headers -o custom-columns=":metadata.name" | xargs -I p watch -n 1 "{ kubectl logs p; } | tail -n 25"