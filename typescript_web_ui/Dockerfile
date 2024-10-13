FROM node:16.14-slim

WORKDIR /app

# Copy package.json and yarn.lock files
COPY package*.json yarn*.lock ./

# Install dependencies
RUN yarn install --frozen-lockfile --production

# Copy the rest of the app code
COPY . .

# Expose the port
EXPOSE 3000

# Start the app
CMD ["yarn", "start"]
