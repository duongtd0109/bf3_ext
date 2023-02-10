#!/bin/bash
kubectl get pods --no-headers -o custom-columns=":metadata.name" | xargs kubectl delete pod